# theanoDemo
Demo of theano on tinis

---
I suggest adding the following to your `.bashrc` file.
```
module load GCC/4.9.3-binutils-2.25
module load Python/2.7.11
```

Download Theano with the following command, which should also give you numpy and scipy
```
pip2.7 install --user Theano
```

---

`g++` (once we have the above modules loaded) is a compiler which works, except for the fact that we have to wait a few seconds after it returns for it to be done. 
We create our own wrapper for it by going into the mycompiler directory and typing 
```
bash compile.sh
```

It doesn't really matter that the compilation is slowed down more than necessary (every single call to g++ is delayed, even those which are independent), because runtime is typically so much greater than compile time. I'm interested in training models for hours, so extra minutes compiling is ok. 

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
python2.7 cpu.py  

```

