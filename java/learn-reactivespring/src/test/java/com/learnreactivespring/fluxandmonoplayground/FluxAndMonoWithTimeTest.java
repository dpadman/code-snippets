package com.learnreactivespring.fluxandmonoplayground;

import org.junit.Test;
import reactor.core.publisher.Flux;
import reactor.test.StepVerifier;

import java.time.Duration;

public class FluxAndMonoWithTimeTest {
	@Test
	public void infiniteSequence() {
		Flux<Long> infiniteFlux = Flux.interval(Duration.ofMillis(100))
				.log(); // long value start from 0 -> ...

		infiniteFlux.subscribe((element) -> System.out.println("Value is : " + element));
		try {
			Thread.sleep(3000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	@Test
	public void infiniteSequenceTest() {
		Flux<Long> finiteFlux = Flux.interval(Duration.ofMillis(100))
				.take(3)
				.log(); // long value start from 0 -> ...
		//finiteFlux.subscribe((element) -> System.out.println("Value is : " + element)); // this creates 2 subscriptions

		StepVerifier.create(finiteFlux)
				.expectSubscription()
				.expectNext(0L, 1L, 2L)
				.verifyComplete();
	}

	@Test
	public void infiniteSequenceMap() {
		Flux<Integer> finiteFlux = Flux.interval(Duration.ofMillis(100))
				.map(l -> new Integer(l.intValue()))
				.take(3)
				.log(); // long value start from 0 -> ...
		//finiteFlux.subscribe((element) -> System.out.println("Value is : " + element)); // this creates 2 subscriptions

		StepVerifier.create(finiteFlux)
				.expectSubscription()
				.expectNext(0, 1, 2)
				.verifyComplete();
	}

	@Test
	public void infiniteSequenceMap_WithDelay() {
		Flux<Integer> finiteFlux = Flux.interval(Duration.ofMillis(100))
				.delayElements(Duration.ofSeconds(1))
				.map(l -> new Integer(l.intValue()))
				.take(3)
				.log(); // long value start from 0 -> ...
		//finiteFlux.subscribe((element) -> System.out.println("Value is : " + element)); // this creates 2 subscriptions

		StepVerifier.create(finiteFlux)
				.expectSubscription()
				.expectNext(0, 1, 2)
				.verifyComplete();
	}
}