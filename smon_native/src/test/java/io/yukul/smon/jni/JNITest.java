package io.yukul.smon.jni;

import org.junit.Test;

public class JNITest {

	@Test
	public void test() {
		/*
		class Callback implements JNICallback {

			@Override
			public void onCallback() {
				//System.out.println("--------------------------------------------------------------------------------------------------------------------------");
				//System.out.println("----------------------------------------------------------------------------------------------------------------onCallbcak");
				
				JNI jni = JNI.getInstance();

				System.out.println("cpu : " + jni.getCpu().toString());
				System.out.println("mem : " + jni.getMem().toString());
				System.out.println("sys : " + jni.getSys().toString());
				System.out.println("disk :\n" + jni.getDisk().toString());
				System.out.println("net :\n" + jni.getNet().toString());
				//for (String str : jni.getProcList()) System.out.println("" + str);
				//System.out.println("\n\nprocs :\n" + jni.getProc().toString());

			}
			
		}
		
		
		Callback cb = new Callback();
		
		JNI jni = JNI.getInstance();
		jni.addCallback(cb);
		jni.start();
		
		jni.registProc("chrome");
		
	
		try {
			while (true) Thread.sleep(100000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

		System.out.println("done!!!");
		jni.stop();
		*/
	}
	

}
