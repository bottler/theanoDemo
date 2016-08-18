# theanoDemo
Demo of theano on tinis

---
Enable python (every session) with one of the following two lines. You cannot have both. You might want to add it to your `.bashrc` file.

```
module load intel impi Python CUDA #for Python 2.7
module load intel impi Python/3.5.1 CUDA # for Python 3.5
```
---
Download Theano with the following command, which should also give you numpy and scipy. You might prefer to call the command as `pip2.7` or `pip3.5` to be sure you get the right version. You don't want to use `python2.6` or `pip2.6`, which `python` and `pip` link to before any other python has been loaded.
```
pip install --user Theano
```
---
I recommend making a modification inside theano's source to stop it trying to figure out if it can use MPI. This check process behaves strangely (at least for me) on the login node and main calculation nodes of tinis. It doesn't really matter if you only ever try to use the GPU nodes. In the file `~/.local/lib/python3.5/site-packages/theano/tensor/io.py` (replace `3.5` with `2.7` if you need to) you'll find a block like this about 100 lines down:
```
try:
    from mpi4py import MPI
except ImportError:
```
Modify this block so the import always fails, e.g. by replacing the import statement with
```
    from mpi4py import MPI_WELOVETINIS
```

#Using icpc

`icpc` is the recommended C++ compiler on tinis. It seems to work OK with theano. 
---
There are warnings about cxxflags meaning that optimizations are not properly turned on. Basically theano doesn't rely on `-march=native` because it wants to cache binaries on the filesystem between uses, keyed by i.a. the compiler flags, and you might use more than one system setup with the same shared working directory. But theano can't correctly detect the native flags with icpc. You can add `gcc.cxxflags=-march=native` to your theano flags to turn off this behaviour without losing the goodness of `-march=native` (I haven't tested, but in practice, this goodness may not add up to much on our 64 bit architecture anyway), but you might want to be careful about the cache if you, e.g., are doing calculations both on  the cnode and gpu queues. Doing this comes with some warnings, which you might want to disable. 

To do that, you'd proceed as follows. Edit the file `~/.local/lib/python3.5/site-packages/theano/gof/cmodule.py` (replace `3.5` with `2.7` if you need to) around line 1820 from 
```
        if ('g++' not in theano.config.cxx and
                'clang++' not in theano.config.cxx and
                'clang-omp++' not in theano.config.cxx):
```
to
```
        if (detect_march and 'g++' not in theano.config.cxx and
                'clang++' not in theano.config.cxx and
                'clang-omp++' not in theano.config.cxx):
```
(on current dev theano, but not any release version, this will look slightly different. There's an extra line about `icpc` in this `if` condition which looks spurious to me. As of 18 August 2016, dev theano doesn't detect `icpc` architecture flags correctly.)

About 9 lines up from this is the main place where the warning telling us not to do what we're doing appears. Change 
```
                    _logger.warn(
```
to 
```
                    _logger.info(
```
to make it quieter.
---
I might set something like 
```
os.environ["THEANO_FLAGS"]="floatX=float32,dnn.enabled=False,device=gpu0,cxx=icpc,gcc.cxxflags=-march=native"
``` 
`cpuIntel.py` demonstrates using `icpc` without GPUs.

There is a choice between using a full node and `msub`, where you control which GPUs you access with 
`CUDA_VISIBLE_DEVICES`, and a method I don't understand yet with `srun` and `sbatch` where it's automatic.

---
#Using GCC
**Important**: The following and most of the rest of this repository documents some attempts to make theano work on tinis with the g++ compiler. The main difficulty is the need to wait after g++ returns before using its binary output, which is an unusual problem.

You might need to load a module for binutils as well as for GCC.

We then hackishly edit Theano to make it wait after ever calling the compiler.
Add the following line to the beginning (after the doc) of the function `dlimport` in the file `~/.local/lib/python2.7/site-packages/theano/gof/cmodule.py`
```
   time.sleep(5)
```
In the current theano (0.8) this will be line 266.
Calling compile() on a keras model seems to call this function about 160 times, so the delay is about 13 minutes.

*Suggested alternative*
When g++ writes to standard output, it doesn't seem to have the timing problem. For example the following works
```
rm -f a.out && g++ a.cpp -o- > a.out && chmod +x a.out && ./a.out
```
I would think it possible to change `cmodule.py` so that it always collected its g++ output from standard output.

---
Up till this point is just setup, and I think it will work either on the head node or on the gpu node. Communicating with github etc. is easier on the head node.

So you want to be in a gpu node session, so type
```
msub -I -q gpu -l walltime=08:00:00
```

(I suggest creating an alias or script for this, I do it often.)

---
To run the demonstration of theano type
```
python gpu.py  
```
This does almost no work but demonstrates theano working with a gpu.
Note that to choose the gpu to use, you have two controls - the `gpu0` in `THEANO_CONFIG` tells theano to use the first GPU, which means the gpu listed first in `CUDA_VISIBLE_DEVICES`.

