package io.yukul.smon.server;

import java.util.TreeSet;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.handler.annotation.MessageMapping;
import org.springframework.messaging.handler.annotation.SendTo;
import org.springframework.stereotype.Controller;

import io.yukul.smon.jni.dto.SysData;

@Controller
public class SmonController {

	@Autowired
	SmonService service;
	
    @MessageMapping("/sys")
    @SendTo("/smon-sub/sys")
    public SysData sys() throws Exception {
    	return service.getJNI().getSys();
    }
    
    @MessageMapping("/proc/list")
    @SendTo("/smon-sub/proc/list")
    public String[] procList() throws Exception {
		TreeSet<String> treeset = service.getJNI().getProcList();
		String[] array = treeset.toArray(new String[treeset.size()]);
		return array;
    }

    @MessageMapping("/proc/regist")
    public void registProc(String processName) throws Exception {
    	service.getJNI().registProc(processName);
    }

    @MessageMapping("/proc/unregist")
    public void unregistProc(String processName) throws Exception {
    	service.getJNI().unregistProc(processName);
    }
    
}
