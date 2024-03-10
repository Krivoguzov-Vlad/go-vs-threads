package main

// #include "timetctxsw2.h"
import "C"

import (
	"flag"
	"fmt"
	"log"
	"runtime"
	"time"
)

func loop(n int) {
	for range n {
		runtime.Gosched()
	}
}

func main() {
	ctxSwitchNPtr := flag.Int("n", 1000000, "number of context switches")
	flag.Parse()
	ctxSwitchN := *ctxSwitchNPtr

	runtime.GOMAXPROCS(1)

	go loop(ctxSwitchN / 2)
	t1 := time.Now()
	loop(ctxSwitchN / 2)
	goCtxSwitch := time.Since(t1) / time.Duration(ctxSwitchN)
	fmt.Printf("Goroutine context switch: %s\n", goCtxSwitch)

	t1 = time.Now()
	if exitCode := C.thread_loop(C.int(ctxSwitchN)); exitCode != 0 {
		log.Fatalf("thread loop exit: %d", exitCode)
	}
	threadCtxSwitch := time.Since(t1) / time.Duration(ctxSwitchN)
	fmt.Printf("OS thread context switch: %s (%.2f%%)\n", threadCtxSwitch, 100*float64(threadCtxSwitch)/float64(goCtxSwitch))
}
