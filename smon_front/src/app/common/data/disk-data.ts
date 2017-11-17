

export interface DiskUsage {
    total: number;
    use: number;
    ioTotal: number;
    ioRead: number;
    ioWrite: number;
}

export interface DiskData {
    utc: number;
    disk: {[key: string]: DiskUsage};
}

