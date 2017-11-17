package io.yukul.smon.jni.dto;

public class DiskUsage {
	private long total;
	private long use;
	private long ioTotal;
	private long ioRead;
	private long ioWrite;
	
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
	public long getIoTotal() {
		return ioTotal;
	}
	public void setIoTotal(long ioTotal) {
		this.ioTotal = ioTotal;
	}
	public long getIoRead() {
		return ioRead;
	}
	public void setIoRead(long ioRead) {
		this.ioRead = ioRead;
	}
	public long getIoWrite() {
		return ioWrite;
	}
	public void setIoWrite(long ioWrite) {
		this.ioWrite = ioWrite;
	}
	
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("total: ").append(total).append(", ");
		sb.append("use: ").append(use).append(", ");
		sb.append("ioTotal: ").append(ioTotal).append(", ");
		sb.append("ioRead: ").append(ioRead).append(", ");
		sb.append("ioWrite: ").append(ioWrite).append(", ");
		return sb.toString();
	}
	
}
