package io.yukul.smon.jni.dto;

public class NetUsage {
	private long total;
	private long sent;
	private long recved;
	
	public long getTotal() {
		return total;
	}
	public void setTotal(long total) {
		this.total = total;
	}
	public long getSent() {
		return sent;
	}
	public void setSent(long sent) {
		this.sent = sent;
	}
	public long getRecved() {
		return recved;
	}
	public void setRecved(long recved) {
		this.recved = recved;
	}

	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("total: ").append(total).append(", ");
		sb.append("sent: ").append(sent).append(", ");
		sb.append("recved: ").append(recved).append(", ");
		return sb.toString();
	}
	
}
