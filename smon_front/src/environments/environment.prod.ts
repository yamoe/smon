export const environment = {
  production: true,
  url : {
    websocket: {
      base : {
        server: '/smon-websocket',
        subscribe: '/smon-sub',
        publish: '/smon-pub'
      },
      path : {
        cpu: '/cpu',
        disk: '/disk',
        mem: '/mem',
        net: '/net',
        sys: '/sys',
        proc: '/proc',
        procList: '/proc/list',
        procRegist: '/proc/regist',
        procUnregist: '/proc/unregist'
      }
    }
  }
};
