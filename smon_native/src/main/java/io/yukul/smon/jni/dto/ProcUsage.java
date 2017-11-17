package io.yukul.smon.jni.dto;

public class ProcUsage {
    private int pid;
    private int cpu;
    private long pmem;
    private long vmem;
    private long ioTotal;
    private long ioRead;
    private long ioWrite;
    private int thCnt; // thread count
	private int tcpCnt;
    private int tcpListenCnt;
    private int tcpEstabCnt;
    private int udpCnt;

    public int getPid() {
		return pid;
	}
	public void setPid(int pid) {
		this.pid = pid;
	}
	public int getCpu() {
		return cpu;
	}
	public void setCpu(int cpu) {
		this.cpu = cpu;
	}
	public long getPmem() {
		return pmem;
	}
	public void setPmem(long pmem) {
		this.pmem = pmem;
	}
	public long getVmem() {
		return vmem;
	}
	public void setVmem(long vmem) {
		this.vmem = vmem;
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
	public int getThCnt() {
		return thCnt;
	}
	public void setThCnt(int thCnt) {
		this.thCnt = thCnt;
	}
	public int getTcpCnt() {
		return tcpCnt;
	}
	public void setTcpCnt(int tcpCnt) {
		this.tcpCnt = tcpCnt;
	}
	public int getTcpListenCnt() {
		return tcpListenCnt;
	}
	public void setTcpListenCnt(int tcpListenCnt) {
		this.tcpListenCnt = tcpListenCnt;
	}
	public int getTcpEstabCnt() {
		return tcpEstabCnt;
	}
	public void setTcpEstabCnt(int tcpEstabCnt) {
		this.tcpEstabCnt = tcpEstabCnt;
	}
	public int getUdpCnt() {
		return udpCnt;
	}
	public void setUdpCnt(int udpCnt) {
		this.udpCnt = udpCnt;
	}
	
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("pid: ").append(pid).append(", ");
		sb.append("cpu: ").append(cpu).append(", ");
		sb.append("pmem: ").append(pmem).append(", ");
		sb.append("vmem: ").append(vmem).append(", ");
		sb.append("ioTotal: ").append(ioTotal).append(", ");
		sb.append("ioRead: ").append(ioRead).append(", ");
		sb.append("ioWrite: ").append(ioWrite).append(", ");
		sb.append("thCnt: ").append(thCnt).append(", ");
		sb.append("tcpCnt: ").append(tcpCnt).append(", ");
		sb.append("tcpListenCnt: ").append(tcpListenCnt).append(", ");
		sb.append("tcpEstabCnt: ").append(tcpEstabCnt).append(", ");
		sb.append("udpCnt: ").append(udpCnt).append(", ");
		return sb.toString();
	}
	
}
