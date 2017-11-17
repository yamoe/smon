import { StompService } from '@stomp/ng2-stompjs';
import { Message } from '@stomp/stompjs';
import { Subscription } from 'rxjs/Subscription';
import { Subject } from 'rxjs/Subject';
import { Observable } from 'rxjs/Rx';

/**
 * Stomp의 queue name 별 메시지 수신
 */

export class WebsocketMessenger<DataT = {}> {
    private messages: Observable<Message>;
    private subscription: Subscription;
    private isSubscribed = false;
    private subject = new Subject<DataT>();

    public constructor(private stomp: StompService, private subscribePath: string, private publishPath: string) {}

    public start() {
        if (this.isSubscribed) return;

        this.messages = this.stomp.subscribe(this.subscribePath);
        this.subscription = this.messages.subscribe(this.onNext);
        this.isSubscribed = true;
    }

    public stop() {
        if (!this.isSubscribed) return;

        this.subscription.unsubscribe();
        this.subscription = null;
        this.messages = null;
        this.isSubscribed = false;
    }

    private onNext = (msg: Message) => {
        this.subject.next(<DataT>JSON.parse(msg.body));
    }

    public subscribe(): Observable<DataT> {
        this.start();
        return this.subject.asObservable();
    }

    public publish(message?): void {
        this.stomp.publish(this.publishPath, message);
    }

}