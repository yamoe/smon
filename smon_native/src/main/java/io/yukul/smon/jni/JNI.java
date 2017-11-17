package io.yukul.smon.jni;

import java.util.TreeSet;

import io.yukul.smon.jni.dto.CpuData;
import io.yukul.smon.jni.dto.DiskData;
import io.yukul.smon.jni.dto.MemData;
import io.yukul.smon.jni.dto.NetData;
import io.yukul.smon.jni.dto.ProcData;
import io.yukul.smon.jni.dto.SysData;

/*
 *  smon_native\bin> javah io.yukul.smon.jni.JNI
 */
public class JNI {

	private static JNI instance = new JNI();
	
	private JNICallback callback;

	private JNI() {}
	
	public static JNI getInstance() {
		return instance;
	}

	public static void onCallback() {
		JNI.getInstance().callbcak();
	}

	public void addCallback(JNICallback callback) {
		this.callback = callback;
		setCallbcak(this.getClass().getName().replace(".", "/"), "onCallback");
	}

	public void callbcak() {
		if (this.callback == null) return;
		this.callback.onCallback();
	}
	
	
	// start native area
	static {
		System.loadLibrary("smon_native");
	}
	
	public native void setPeriod(int sec);
	private native void setCallbcak(String packageName, String methodName);
	public native void start();
	public native void stop();
	
    public native CpuData getCpu();
    public native MemData getMem();
    public native SysData getSys();
    public native DiskData getDisk();
    public native NetData getNet();
    public native TreeSet<String> getProcList();
    public native void registProc(String name);
    public native void unregistProc(String name);
    public native ProcData getProc();
	
}

