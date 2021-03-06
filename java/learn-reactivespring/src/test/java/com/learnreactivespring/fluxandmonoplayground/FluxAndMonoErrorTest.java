package com.learnreactivespring.fluxandmonoplayground;

import java.time.Duration;

import org.junit.Test;
import reactor.core.publisher.Flux;
import reactor.test.StepVerifier;

public class FluxAndMonoErrorTest {

	@Test
	public void fluxErrorHandling() {
		Flux<String> stringFlux = Flux.just("A", "B", "C")
				.concatWith(Flux.error(new RuntimeException("Exception Occurred")))
				.concatWith(Flux.just("D"))
				.onErrorResume((e) -> { // this block gets executed and returns flux
					System.out.println("Exception is : " + e);
					return Flux.just("default1", "default2");
				});

		StepVerifier.create(stringFlux.log())
				.expectSubscription()
				.expectNext("A", "B", "C")
				/*.expectError(RuntimeException.class).
				verify();*/
				.expectNext("default1", "default2")
				.verifyComplete();
	}

	@Test
	public void fluxErrorHandling_OnErrorReturn() {
		Flux<String> stringFlux = Flux.just("A", "B", "C")
				.concatWith(Flux.error(new RuntimeException("Exception Occurred")))
				.concatWith(Flux.just("D"))
				.onErrorReturn("default");

		StepVerifier.create(stringFlux.log())
				.expectSubscription()
				.expectNext("A", "B", "C")
				.expectNext("default")
				.verifyComplete();
	}

	@Test
	public void fluxErrorHandling_OnErrorMap() {
		Flux<String> stringFlux = Flux.just("A", "B", "C")
				.concatWith(Flux.error(new RuntimeException("Exception Occurred")))
				.concatWith(Flux.just("D"))
				.onErrorMap((e) -> new CustomException(e));

		StepVerifier.create(stringFlux.log())
				.expectSubscription()
				.expectNext("A", "B", "C")
				.expectError(CustomException.class)
				.verify();
	}

	@Test
	public void fluxErrorHandling_OnErrorMap_WithRetry() {
		Flux<String> stringFlux = Flux.just("A", "B", "C")
				.concatWith(Flux.error(new RuntimeException("Exception Occurred")))
				.concatWith(Flux.just("D"))
				.onErrorMap((e) -> new CustomException(e))
				.retry(2);

		StepVerifier.create(stringFlux.log())
				.expectSubscription()
				.expectNext("A", "B", "C", "A", "B", "C", "A", "B", "C")
				.expectError(CustomException.class)
				.verify();
	}

	@Test
	public void fluxErrorHandling_OnErrorMap_WithRetryBackoff() {
		Flux<String> stringFlux = Flux.just("A", "B", "C")
				.concatWith(Flux.error(new RuntimeException("Exception Occurred")))
				.concatWith(Flux.just("D"))
				.onErrorMap((e) -> new CustomException(e))
				.retryBackoff(2, Duration.ofSeconds(2));

		StepVerifier.create(stringFlux.log())
				.expectSubscription()
				.expectNext("A", "B", "C", "A", "B", "C", "A", "B", "C")
				.expectError(IllegalStateException.class)
				.verify();
	}
}