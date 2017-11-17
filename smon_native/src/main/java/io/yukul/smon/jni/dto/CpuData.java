package io.yukul.smon.jni.dto;

import java.text.SimpleDateFormat;
import java.util.ArrayList;

public class CpuData {
	private long utc = 0;
	private int total = 0;
	private ArrayList<Integer> cores = new ArrayList<Integer>();
	

	public long getUtc() {
		return utc;
	}
	public void setUtc(long utc) {
		this.utc = utc;
	}	
	public int getTotal() {
		return total;
	}
	public void setTotal(int total) {
		this.total = total;
	}	
	public ArrayList<Integer> getCores() {
		return cores;
	}
	public void setCores(ArrayList<Integer> cores) {
		this.cores = cores;
	}
	
	public String toString()
	{
		StringBuilder sb = new StringBuilder();
		
		sb.append((new SimpleDateFormat("(yyyy-MM-dd HH:mm:ss) ")).format(new java.util.Date(utc * 1000)));
		sb.append(total).append(" - ");
		
		int size = cores.size();
		for (int i=0; i<size; ++i) {
			sb.append(cores.get(i));
			if ((i+1) != size) {
				sb.append(", ");
			}
		}
		return sb.toString();
	}
}
