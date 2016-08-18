# theanoDemo
Demo of theano on tinis

---
Enable python (every session) with one of the following two lines. You cannot have both. I suggest adding the following to your `.bashrc` file.

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

`icpc` is the recommended C++ compiler on tinis. 
It seems to work OK with theano. 
Just ignore the warnings about cxxflags. (Or comment them out in the theano source).
I might set something like 
```
os.environ["THEANO_FLAGS"]="floatX=float32,dnn.enabled=False,device=gpu0,cxx=icpc"
``` 

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
Up till this point is just setup, and I think it will work either on the head node or on the gpu node. It is a bad idea, I don't totally understand why, to try to use theano, even trivially, on the head node - you get weird errors. However, communicating with github etc. is easier on the head node.

So you want to be in a gpu node session, so type
```
msub -I -q gpu -l walltime=08:00:00
```

(I suggest creating an alias or script for this, I do it often.)

---
To run the demonstration of theano type
```
python2.7 gpu.py  
```
This does almost no work but demonstrates theano working with a gpu.
Note that to choose the gpu to use, you have two controls - the `gpu0` in `THEANO_CONFIG` tells theano to use the first GPU, which means the gpu listed first in `CUDA_VISIBLE_DEVICES`.

