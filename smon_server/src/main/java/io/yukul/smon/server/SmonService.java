package io.yukul.smon.server;

import javax.annotation.PostConstruct;
import javax.annotation.PreDestroy;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.simp.SimpMessagingTemplate;
import org.springframework.stereotype.Service;

import io.yukul.smon.jni.JNI;
import io.yukul.smon.jni.JNICallback;

@Service
public class SmonService implements JNICallback {

	JNI jni = JNI.getInstance();
	
	@Autowired
	private SimpMessagingTemplate template;


	@Override
	public void onCallback() {
		template.convertAndSend("/smon-sub/cpu", jni.getCpu());
		template.convertAndSend("/smon-sub/disk", jni.getDisk());
		template.convertAndSend("/smon-sub/mem", jni.getMem());
		template.convertAndSend("/smon-sub/net", jni.getNet());
		template.convertAndSend("/smon-sub/proc", jni.getProc());
	}	

	@PostConstruct
	public void PostConstruct() {
		jni.addCallback(this);
		jni.setPeriod(1);
		jni.start();
	}

	@PreDestroy
	public void preDestroy() {
		jni.stop();
	}

	public JNI getJNI() {
		return jni;
	}

}
