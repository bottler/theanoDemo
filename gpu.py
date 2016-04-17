import os,time
os.environ["CUDA_VISIBLE_DEVICES"]="0,1,2,3"
#os.environ["CUDA_VISIBLE_DEVICES"]="0,2,3"
#os.environ["THEANO_FLAGS"]="cxx="
#os.environ["THEANO_FLAGS"]="cxx=./myg++"
#os.environ["THEANO_FLAGS"]="cxx=./myg++,floatX=float32,dnn.enabled=False,device=gpu0"
os.environ["THEANO_FLAGS"]="floatX=float32,dnn.enabled=False,device=gpu0"
#os.environ["THEANO_FLAGS"]="floatX=float32,device=gpu0"
import theano

#mydlimport1 = theano.gof.cmodule.dlimport
#def mydlimport (fullpath, suffix=None):
#	time.sleep(5)
#	print "***HI!!!"
#	return mydlimport1(fullpath, suffix)
#theano.gof.cmodule.dlimport = mydlimport

a=theano.tensor.fscalar()
b=theano.function([a],[a*a])

print "five times five is:", b(5.0)

