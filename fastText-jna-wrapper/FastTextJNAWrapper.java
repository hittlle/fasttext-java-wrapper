import java.util.Iterator;
import java.util.concurrent.ConcurrentLinkedDeque;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.DoubleByReference;

public class FastTextJNAWrapper {
	public interface CLibrary extends Library {
		static String FASTTEXT_SO_NAME = "fasttext";
		//CLibrary instance = (CLibrary)Native.load(Platform.isWindows()?"fasttext":"fasttext", CLibrary.class);
		/*
			only support *iux currently, assume that the SO name is libfasttext.so, and you've copied it to 
			/lib or /usr/local/lib or any other path pointed to by the environment variable LD_LIBRARY_PATH
		*/
		CLibrary instance = (CLibrary)Native.load(FASTTEXT_SO_NAME, CLibrary.class);
		/**
		 * Wrapper of C functions in JNA
		 * C void* -> JNA Pointer
		 * C char* function argument -> Java byte[]
		 * C const char* function argument -> Java String
		 * C double* function argument -> JNA DoubleByReference
		 * C int* function argument -> JNA IntByReference
		 */
		/**
		 * Create FastText instance 
		 * @return Pointer to FastText instance
		 */
		Pointer getInstance();
		/**
		 * Load model file(model.bin, etc) generated by training using commands like
		 * 		./fasttext supervised -input data.train -output model.bin -lr 0.35 -epoch 40 -loss hs
		 * If you are not familiar with this part, please go to https://fasttext.cc/ to get detailed information.
		 * @param handler pointer returned by getInstance()
		 * @param path path to the model file
		 * @param size length of path
		 */
		void prepareModel(Pointer handler,  byte[] path, int size);
		/**
		 * predict the class of a given sentence
		 * @param handler pointer returned by getInstance()
		 * @param sentence the sentence to predict
		 * @param label used to carry the predicted class
		 * @param labelSize the size/capacity of byte array label
		 * @param score the confidence of the predicted class
		 */
		void predictSentence(Pointer handler, String sentence, byte[] label,int labelSize, DoubleByReference score);
	}
	public static void main(String[] args) throws Exception {
		String model = "model.bin";
		//step 1: create the handler
		Pointer handler = CLibrary.instance.getInstance();
		if (null != handler) {
			//step 2: load the model file
			CLibrary.instance.prepareModel(handler, model.getBytes(), model.getBytes().length);
			//step 3: predict the class of a given sentence
			String sentences[] = {
				"I don't think she is clever, she is a jerk",
				"Wow, that's great"
			};
			byte []label = new byte[20];
			DoubleByReference score = new DoubleByReference();
			for (String sentence: sentences) {
				CLibrary.instance.predictSentence(handler, sentence, label, 20, score);
				System.out.println(String.format("sentence: %s. label: %s. confidence:%f", sentence, new String(label), score.getValue()));
			}
		}
	}
}
