

export interface ProcUsage {
    pid: number;
    cpu: number;
    pmem: number;   // physical memory (Working Set)
    vmem: number;   // virtual memory (Private Bytes)
    ioTotal: number;
    ioRead: number;
    ioWrite: number;
    thCnt: number;  // thread count
	tcpCnt: number; // tcp total count
    tcpListenCnt: number;
    tcpEstabCnt: number;
    udpCnt: number; // udp total count
}

export interface ProcData {
    utc: number;
    proc: {[key: string]: ProcUsage[]};
}

