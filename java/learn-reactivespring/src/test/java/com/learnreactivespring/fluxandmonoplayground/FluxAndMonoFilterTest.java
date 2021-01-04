package com.learnreactivespring.fluxandmonoplayground;

import org.junit.Test;
import java.util.List;
import java.util.Arrays;

import reactor.core.publisher.Flux;
import reactor.test.StepVerifier;

public class FluxAndMonoFilterTest {
	List<String> names = Arrays.asList("Adam", "Anna", "Jack", "Jenny");

	@Test
	public void filterTest() {
		Flux<String> namesFlux = Flux.fromIterable(names)
				.filter(s -> s.startsWith("A"))
				.log();

		StepVerifier.create(namesFlux)
				.expectNext("Adam", "Anna")
				.verifyComplete();
	}

	@Test
	public void filterTestLength() {
		Flux<String> namesFlux = Flux.fromIterable(names)
				.filter(s -> s.length() > 4)
				.log();

		StepVerifier.create(namesFlux)
				.expectNext("Jenny")
				.verifyComplete();
	}
}