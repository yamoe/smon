import { RouteReuseStrategy, ActivatedRouteSnapshot, DetachedRouteHandle } from "@angular/router";

/*
호출 관계 테스트
    1. /proc 열기
        shouldReuseRoute. future: /, curr : /
        retrieve : /
        retrieve : /
        retrieve : /proc/
        retrieve : /proc/
        shouldAttach : /
        shouldAttach : /proc/
    2. /mem 이동
        shouldReuseRoute. future: /, curr : /
        shouldReuseRoute. future: /, curr : /
        shouldReuseRoute. future: /, curr : /
        shouldReuseRoute. future: /, curr : /
        shouldReuseRoute. future: /, curr : /
        shouldReuseRoute. future: /proc/, curr : /mem/
        retrieve : /mem/
        retrieve : /mem/
        shouldDetach : /proc/
        store : /proc/
        shouldAttach : /mem/
    3. /net 이동
        shouldReuseRoute. future: /, curr : /
        shouldReuseRoute. future: /, curr : /
        shouldReuseRoute. future: /, curr : /
        shouldReuseRoute. future: /, curr : /
        shouldReuseRoute. future: /, curr : /
        shouldReuseRoute. future: /mem/, curr : /net/
        retrieve : /net/
        retrieve : /net/
        shouldDetach : /mem/
        store : /mem/
        shouldAttach : /net/
    4. /mem 로 다시 이동
        shouldReuseRoute. future: /, curr : /
        shouldReuseRoute. future: /, curr : /
        shouldReuseRoute. future: /, curr : /
        shouldReuseRoute. future: /, curr : /
        shouldReuseRoute. future: /, curr : /
        shouldReuseRoute. future: /net/, curr : /mem/
        retrieve : /mem/
        retrieve : /mem/
        shouldDetach : /net/
        store : /net/
        shouldAttach : /mem/
        retrieve : /mem/
        store : /mem/
*/
export class CustomReuseStrategy implements RouteReuseStrategy {

    private handlers: { [key: string]: DetachedRouteHandle } = {};

    public retrieve(route: ActivatedRouteSnapshot): DetachedRouteHandle {
        let key = this.calcKey(route);
        //console.log('retrieve : ' + key);
        return this.handlers[key];
    }

    public shouldAttach(route: ActivatedRouteSnapshot): boolean {
        let key = this.calcKey(route);
        //console.log('shouldAttach : ' + key);
        return !!this.handlers[key];
    }
    
    public store(route: ActivatedRouteSnapshot, handle: DetachedRouteHandle): void {
        let key = this.calcKey(route);
        //console.log('store : ' + key);
        this.handlers[key] = handle;
    }

    public shouldReuseRoute(future: ActivatedRouteSnapshot, curr: ActivatedRouteSnapshot): boolean {
        //console.log('shouldReuseRoute. future: ' + this.calcKey(future) + ', curr : ' + this.calcKey(curr));
        return future.routeConfig === curr.routeConfig;
    }

    public shouldDetach(route: ActivatedRouteSnapshot): boolean {
        //console.log('shouldDetach : ' + this.calcKey(route));
        return true;
    }

    private calcKey(route: ActivatedRouteSnapshot): string {
        let url = '/';
        for (var i=0; i<route.pathFromRoot.length; ++i) {
            const r = route.pathFromRoot[i];
            if (!r.url) continue;
            for (var j=0; j<r.url.length; ++j) {
                const u = r.url[j];
                if (!u.path) continue;
                url += u.path + '/';
            }
        }
        // console.log(url);
        if (!url.length) return undefined;
        return url;
    }
}   