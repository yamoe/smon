package io.yukul.smon.jni.dto;

import java.util.Map.Entry;
import java.text.SimpleDateFormat;
import java.util.TreeMap;

public class DiskData {
	private long utc = 0;
	private TreeMap<String, DiskUsage> disk;

	public long getUtc() {
		return utc;
	}
	public void setUtc(long utc) {
		this.utc = utc;
	}	
	public TreeMap<String, DiskUsage> getDisk() {
		return disk;
	}

	public void setDisk(TreeMap<String, DiskUsage> disks) {
		this.disk = disks;
	}
	
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append((new SimpleDateFormat("(yyyy-MM-dd HH:mm:ss) ")).format(new java.util.Date(utc * 1000)));
		sb.append("\n");
		for (Entry<String, DiskUsage> it : disk.entrySet()) {
			sb.append(it.getKey()).append(" - ").append(it.getValue().toString()).append("\n");
		}
		return sb.toString();
	}
}
