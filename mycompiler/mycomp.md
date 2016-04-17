
---

`g++` (once we have the above modules loaded) is a compiler which works, except for the fact that we have to wait a few seconds after it returns for it to be done. 
We create our own wrapper for it by going into the mycompiler directory and typing 
```
bash compile.sh
```

It doesn't really matter that the compilation is slowed down more than necessary (every single call to g++ is delayed, even those which are independent), because runtime is typically so much greater than compile time. I'm interested in training models for hours, so extra minutes compiling is ok. 

