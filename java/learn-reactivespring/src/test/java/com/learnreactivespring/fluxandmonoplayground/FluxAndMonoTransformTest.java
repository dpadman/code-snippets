package com.learnreactivespring.fluxandmonoplayground;

import java.util.Arrays;
import java.util.List;

import org.junit.Test;

import reactor.core.publisher.Flux;
import reactor.test.StepVerifier;
import static reactor.core.scheduler.Schedulers.parallel;

public class FluxAndMonoTransformTest {

	List<String> names = Arrays.asList("Adam", "Anna", "Jack", "Jenny");

	@Test
	public void transformUsingMap() {
		Flux<String> namesFlux = Flux.fromIterable(names)
				.map(s -> s.toUpperCase())
				.log();

		StepVerifier.create(namesFlux)
				.expectNext("ADAM", "ANNA", "JACK", "JENNY")
				.verifyComplete();
	}

	@Test
	public void transformUsingMap_Length() {
		Flux<Integer> namesFlux = Flux.fromIterable(names)
				.map(s -> s.length())
				.log();

		StepVerifier.create(namesFlux)
				.expectNext(4, 4, 4, 5)
				.verifyComplete();
	}

	@Test
	public void transformUsingMap_Length_Repeat() {
		Flux<Integer> namesFlux = Flux.fromIterable(names)
				.map(s -> s.length())
				.repeat(1)
				.log();

		StepVerifier.create(namesFlux)
				.expectNext(4, 4, 4, 5, 4, 4, 4, 5)
				.verifyComplete();
	}

	@Test
	public void transformUsingMap_Filter() {
		Flux<String> namesFlux = Flux.fromIterable(names)
				.filter(s -> s.length() > 4)
				.map(s -> s.toUpperCase())
				.log();

		StepVerifier.create(namesFlux)
				.expectNext("JENNY")
				.verifyComplete();
	}

	private List<String> convertToList(String s) {
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		return Arrays.asList(s, "newValue");
	}

	@Test
	public void transformUsingFlatMap() {
		Flux<String> stringFlux = Flux.fromIterable(Arrays.asList("A", "B", "C", "D", "E", "F"))
				.flatMap(s -> {
					return Flux.fromIterable(convertToList(s));
				})
				.log();

		StepVerifier.create(stringFlux)
				.expectNextCount(12)
				.verifyComplete();
	}

	@Test
	public void transformUsingFlatMap_UsingParallel() {
		Flux<String> stringFlux = Flux.fromIterable(Arrays.asList("A", "B", "C", "D", "E", "F"))
				.window(2)	// Flux<Flux<String> -> (A,B), (C,D), (E,F)
				.flatMap((s) ->
					s.map(this::convertToList).subscribeOn(parallel())) // Flux<List<String>
				.flatMap(s -> Flux.fromIterable(s))	// Flux<String>
				.log();

		StepVerifier.create(stringFlux)
				.expectNextCount(12)
				.verifyComplete();
	}

	@Test
	public void transformUsingFlatMap_Parallel_Maintain_Order() {
		Flux<String> stringFlux = Flux.fromIterable(Arrays.asList("A", "B", "C", "D", "E", "F"))
				.window(2)	// Flux<Flux<String> -> (A,B), (C,D), (E,F)
				/* .concatMap((s) ->
					s.map(this::convertToList).subscribeOn(parallel())) */ // no order Flux<List<String>
				.flatMapSequential((s) ->
					s.map(this::convertToList).subscribeOn(parallel())) // Flux<List<String>
				.flatMap(s -> Flux.fromIterable(s))	// Flux<String>
				.log();

		StepVerifier.create(stringFlux)
				.expectNextCount(12)
				.verifyComplete();
	}
}