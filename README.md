# theanoDemo
Demo of theano on tinis

---
I suggest adding the following to your `.bashrc` file.
```
#module load GCC/4.9.3-binutils-2.25
#module load Python/2.7.11
module load intel impi Python CUDA
```

Download Theano with the following command, which should also give you numpy and scipy
```
pip2.7 install --user Theano
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

