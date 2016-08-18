import os
#os.environ["THEANO_FLAGS"]="cxx="
os.environ["THEANO_FLAGS"]="cxx=icpc,gcc.cxxflags=-march=native"
#os.environ["THEANO_FLAGS"]="cxx=./myg++,floatX=float32,device=gpu0"

import theano

a=theano.tensor.fscalar()
b=theano.function([a],[a*a])

print ("five times five is:", b(5.0))

