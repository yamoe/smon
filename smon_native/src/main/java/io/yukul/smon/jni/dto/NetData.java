package io.yukul.smon.jni.dto;

import java.util.TreeMap;
import java.text.SimpleDateFormat;
import java.util.Map.Entry;

public class NetData {
	private long utc = 0;
	private TreeMap<String, NetUsage> net;

	public long getUtc() {
		return utc;
	}
	public void setUtc(long utc) {
		this.utc = utc;
	}
	
	public TreeMap<String, NetUsage> getNet() {
		return net;
	}
	public void setNet(TreeMap<String, NetUsage> net) {
		this.net = net;
	}


	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append((new SimpleDateFormat("(yyyy-MM-dd HH:mm:ss) ")).format(new java.util.Date(utc * 1000)));
		sb.append("\n");
		for (Entry<String, NetUsage> it : net.entrySet()) {
			sb.append(it.getKey()).append(" - ").append(it.getValue().toString()).append("\n");
		}
		return sb.toString();
	}
}
