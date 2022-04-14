package main

import "testing"

func BenchmarkPhilo(b *testing.B) {
	for i := 0; i < b.N; i++ {
		mid()
	}
}
