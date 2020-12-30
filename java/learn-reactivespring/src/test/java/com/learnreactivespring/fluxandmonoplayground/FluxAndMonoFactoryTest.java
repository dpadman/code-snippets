package com.learnreactivespring.fluxandmonoplayground;

import org.junit.Test;

import jdk.jfr.Timestamp;
import reactor.core.publisher.Flux;
import reactor.core.publisher.Mono;
import reactor.test.StepVerifier;

import java.util.List;
import java.util.function.Supplier;
import java.util.Arrays;

public class FluxAndMonoFactoryTest {

	List<String> names = Arrays.asList("Adam", "Anna", "Jack", "Jenny");

	@Test
	public void fluxUsingIterable() {
		Flux<String> namesFlux = Flux.fromIterable(names);

		StepVerifier.create(namesFlux.log())
				.expectNext("Adam", "Anna", "Jack", "Jenny")
				.verifyComplete();
	}

	@Test
	public void fluxUsingArray() {
		String[] names = new String[] {"Adam", "Anna", "Jack", "Jenny"};
		Flux<String> namesFlux = Flux.fromArray(names).log();
		StepVerifier.create(namesFlux)
				.expectNext("Adam", "Anna", "Jack", "Jenny")
				.verifyComplete();
	}

	@Test
	public void fluxUsingStream() {
		Flux<String> namesFlux = Flux.fromStream(names.stream()).log();

		StepVerifier.create(namesFlux)
				.expectNext("Adam", "Anna", "Jack", "Jenny")
				.verifyComplete();
	}

	@Test
	public void monoUsingJustOrEmpty() {
		Mono<String> mono = Mono.justOrEmpty(null);	// Mono.empty

		StepVerifier.create(mono.log())
				.verifyComplete();
	}

	@Test
	public void monoUsingSupplier() {
		Supplier<String> stringSupplier = () -> "Adam";
		Mono<String> stringMono = Mono.fromSupplier(stringSupplier);

		System.out.println(stringSupplier.get());
		StepVerifier.create(stringMono.log())
				.expectNext("Adam")
				.verifyComplete();
	}

	@Test
	public void fluxUsingRange() {
		Flux<Integer> integerFlux = Flux.range(1, 5).log();
		StepVerifier.create(integerFlux)
				.expectNext(1, 2, 3, 4, 5)
				.verifyComplete();

	}
}