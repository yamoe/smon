package io.yukul.smon.jni.dto;

import java.text.SimpleDateFormat;

public class SysData {
	private long utc = 0;
    private String hostName;
    private String osName;
    private String processor;
    private String baseboard;
    private String ram;
    private String disk;
    private String ip;

	public long getUtc() {
		return utc;
	}
	public void setUtc(long utc) {
		this.utc = utc;
	}
	public String getHostName() {
		return hostName;
	}
	public void setHostName(String hostName) {
		this.hostName = hostName;
	}
	public String getOsName() {
		return osName;
	}
	public void setOsName(String osName) {
		this.osName = osName;
	}
	public String getProcessor() {
		return processor;
	}
	public void setProcessor(String processor) {
		this.processor = processor;
	}
	public String getBaseboard() {
		return baseboard;
	}
	public void setBaseboard(String baseboard) {
		this.baseboard = baseboard;
	}
	public String getRam() {
		return ram;
	}
	public void setRam(String ram) {
		this.ram = ram;
	}
	public String getDisk() {
		return disk;
	}
	public void setDisk(String disk) {
		this.disk = disk;
	}
	public String getIp() {
		return ip;
	}
	public void setIp(String ip) {
		this.ip = ip;
	}

	public String toString()
	{
		StringBuilder sb = new StringBuilder();
		sb.append((new SimpleDateFormat("(yyyy-MM-dd HH:mm:ss) ")).format(new java.util.Date(utc * 1000)));
		sb.append("hostname: ").append(hostName).append(", ");
		sb.append("os: ").append(osName).append(", ");
		sb.append("processor: ").append(processor).append(", ");
		sb.append("baseboard: ").append(baseboard).append(", ");
		sb.append("ram: ").append(ram).append(", ");
		sb.append("disk: ").append(disk).append(", ");
		sb.append("ip: ").append(ip);
		return sb.toString();
	}
}
