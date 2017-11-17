package io.yukul.smon.jni.dto;

import java.text.SimpleDateFormat;

public class MemData {
	private long utc = 0;
	private long total;
	private long use;

	public long getUtc() {
		return utc;
	}
	public void setUtc(long utc) {
		this.utc = utc;
	}

	public long getTotal() {
		return total;
	}
	
	public void setTotal(long total) {
		this.total = total;
	}
	
	public long getUse() {
		return use;
	}
	
	public void setUse(long use) {
		this.use = use;
	}
	

	public String toString()
	{
		StringBuilder sb = new StringBuilder();
		sb.append((new SimpleDateFormat("(yyyy-MM-dd HH:mm:ss) ")).format(new java.util.Date(utc * 1000)));
		sb.append("total: ").append(total).append(", ");
		sb.append("use: ").append(use);
		return sb.toString();
	}
	
}
