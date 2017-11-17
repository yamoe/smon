package io.yukul.smon.jni.dto;

import java.util.TreeMap;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Map.Entry;

public class ProcData {
	private long utc = 0;
	private TreeMap<String, ArrayList<ProcUsage>> proc;

	public long getUtc() {
		return utc;
	}
	public void setUtc(long utc) {
		this.utc = utc;
	}	
	public TreeMap<String, ArrayList<ProcUsage>> getProc() {
		return proc;
	}

	public void setProc(TreeMap<String, ArrayList<ProcUsage>> proc) {
		this.proc = proc;
	}	

	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append((new SimpleDateFormat("(yyyy-MM-dd HH:mm:ss) ")).format(new java.util.Date(utc * 1000)));
		sb.append("\n");
		for (Entry<String, ArrayList<ProcUsage>> it : proc.entrySet()) {
			sb.append(it.getKey()).append(" --\n");
			for (ProcUsage u : it.getValue()) {
				sb.append(u.toString()).append("\n");
			}
		}
		return sb.toString();
	}
	
	
}
