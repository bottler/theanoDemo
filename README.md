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

We then hackishly edit Theano to make it wait after ever calling the compiler.
Add the following line to the beginning (after the doc) of the function `dlimport` in the file `~/.local/lib/python2.7/site-packages/theano/gof/cmodule.py`
```
   time.sleep(5)
```
In the current theano (0.8) this will be line 266.

---
Up till this point is just setup, and I think it will work either on the head node or on the gpu node. It is a bad idea, I don't totally understand why, to try to use theano, even trivially, on the head node - you get weird errors.

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

