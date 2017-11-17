

export interface NetUsage {
    total: number;
    sent: number;
    recved: number;
}

export interface NetData {
    utc: number;
    net: {[key: string]: NetUsage};
}

