package com.learnreactivespring.fluxandmonoplayground;

import java.time.Duration;

import org.junit.Test;
import reactor.core.publisher.Flux;
import reactor.core.publisher.ConnectableFlux;

public class ColdAndHotPublisherTest {

	@Test
	public void coldPublisher() throws InterruptedException {
		Flux<String> stringFlux = Flux.just("A", "B", "C", "D", "E", "F")
				.delayElements(Duration.ofSeconds(1));

		stringFlux.subscribe(s -> System.out.println("Subscriber 1 : " + s));
		Thread.sleep(2000);

		stringFlux.subscribe(s -> System.out.println("Subscriber 2 : " + s));	// emits values from beginning
		Thread.sleep(4000);
	}

	@Test
	public void hotPublisher() throws InterruptedException {
		Flux<String> stringFlux = Flux.just("A", "B", "C", "D", "E", "F")
				.delayElements(Duration.ofSeconds(1));

		ConnectableFlux<String> connectableFlux = stringFlux.publish();
		connectableFlux.connect();

		connectableFlux.subscribe(s -> System.out.println("Subscriber 1 : " + s));
		Thread.sleep(3000);

		connectableFlux.subscribe(s -> System.out.println("Subscriber 2 : " + s));	// does not emit values from beginning
		Thread.sleep(4000);
	}
}