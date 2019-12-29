package main

import (
	"fmt"
	"math"
	"math/rand"
	"time"
)

func inQuad(x, y float64) bool {
	if x*x+y*y <= 1 {
		return true
	}
	return false
}

func getPoint(i int, m chan int) {
	local := 0
	r := rand.New(rand.NewSource(time.Now().UnixNano()))
	for j := 0; j < i; j++ {
		x := r.Float64()
		y := r.Float64()
		if inQuad(x, y) {
			local++
		}
	}
	m <- local
}

func main() {
	sum := 0
	points := 1000000043
	routines := 100
	parts := points / routines
	messages := make(chan int, routines)
	remainder := points % routines
	fmt.Printf("Routines: %d, Points per Routine: %d, Remainder: %d\n", routines, parts, remainder)
	for i := 0; i < routines; i++ {
		go getPoint(parts, messages)
	}
	go getPoint(remainder, messages)
	for i := 0; i <= routines; i++ {
		sum += <-messages
	}
	var pi float64 = float64(sum) * 4 / float64(points)
	fmt.Println(sum)
	fmt.Println(pi)
	fmt.Println(math.Pi)

}
