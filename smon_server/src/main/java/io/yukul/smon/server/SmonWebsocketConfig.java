package io.yukul.smon.server;

import org.springframework.context.annotation.Configuration;
import org.springframework.messaging.simp.config.MessageBrokerRegistry;
import org.springframework.web.socket.config.annotation.AbstractWebSocketMessageBrokerConfigurer;
import org.springframework.web.socket.config.annotation.EnableWebSocketMessageBroker;
import org.springframework.web.socket.config.annotation.StompEndpointRegistry;


@Configuration
@EnableWebSocketMessageBroker
public class SmonWebsocketConfig extends AbstractWebSocketMessageBrokerConfigurer {

	@Override
	public void registerStompEndpoints(StompEndpointRegistry registry) {
		/*
		 * angular(front-end) 를 별도로 띄우는 경우를 위해 
		 * CORS 설정인 setAllowedOrigins 를 추가함
		 */
		registry.addEndpoint("/smon-websocket").setAllowedOrigins("*").withSockJS();
	}
	
	@Override
	public void configureMessageBroker(MessageBrokerRegistry registry) {
		registry.enableSimpleBroker("/smon-sub");	// subscribe from client
		registry.setApplicationDestinationPrefixes("/smon-pub");	// publish from client
	}

}

