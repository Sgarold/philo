package main

import (
	"fmt"
	"log"
	"os"
	"runtime"
	"strconv"
	"sync"
	"time"
)

type Philo struct {
	leftFork    *sync.Mutex
	rightFork   *sync.Mutex
	typingMutex *sync.Mutex
	id          int
	mealsCount  int
	deadSignal  chan int
	timer       time.Time
	table       *Table
}

type Table struct {
	start       time.Time
	philoCount  int
	timeToDead  int
	timeToEat   int
	timeToSleep int
	mealsCount  int
	philos      []Philo
}

func initTable(args []string) Table {
	table := Table{}

	var err error

	table.philoCount, err = strconv.Atoi(args[1])
	if err != nil || table.philoCount < 1 {
		log.Fatalln()
	}
	table.timeToDead, err = strconv.Atoi(args[2])
	if err != nil {
		log.Fatalln(err)
	}
	table.timeToEat, err = strconv.Atoi(args[3])
	if err != nil {
		log.Fatalln(err)
	}
	table.timeToSleep, err = strconv.Atoi(args[4])
	if err != nil {
		log.Fatalln(err)
	}
	table.mealsCount = -1
	if len(args) == 6 {
		table.mealsCount, err = strconv.Atoi(args[5])
		if err != nil {
			log.Fatalln(err)
		}
	}
	table.philos = make([]Philo, table.philoCount)
	table.start = time.Now()
	return table
}

func initPhilos(table *Table) {
	var signal chan int
	typingMutex := &sync.Mutex{}

	forks := make([]sync.Mutex, table.philoCount)

	table.philos = make([]Philo, table.philoCount)

	for i := 0; i < table.philoCount; i++ {
		table.philos[i].rightFork = &(forks[i])
		if i == table.philoCount-1 {
			table.philos[i].leftFork = &forks[0]

		} else {
			table.philos[i].leftFork = &forks[i+1]
		}
		table.philos[i].mealsCount = table.mealsCount
		table.philos[i].typingMutex = typingMutex
		table.philos[i].deadSignal = signal
		table.philos[i].id = i + 1
		table.philos[i].table = table
	}
}

func (p *Philo) typing(str string, mode bool) {
	p.typingMutex.Lock()
	fmt.Printf("| %010v |\tphilo %3d  %-22s|\n", time.Since(p.table.start).Milliseconds(), p.id, str)
	if mode {
		os.Exit(0)
	} else {
		p.typingMutex.Unlock()
	}
}

func (p *Philo) eats() {
	if p.id%2 == 0 {
		p.leftFork.Lock()
		p.typing("has taken left fork", false)
		p.rightFork.Lock()
		p.typing("has taken right fork", false)
	} else {
		p.rightFork.Lock()
		p.typing("has taken right fork", false)
		p.leftFork.Lock()
		p.typing("has taken left fork", false)
	}
	p.timer = time.Now()
	p.typing("starts eating", false)
	time.Sleep(time.Duration(p.table.timeToEat) * time.Millisecond)
	p.leftFork.Unlock()
	p.rightFork.Unlock()
	if p.mealsCount > 0 {
		p.mealsCount--
	}
}

func (p *Philo) sleep() {
	p.typing("starts sleep", false)
	time.Sleep(time.Duration(p.table.timeToSleep) * time.Millisecond)

}

func philoCycle(philo *Philo, wg *sync.WaitGroup) {

	for philo.mealsCount != 0 {
		philo.eats()
		philo.sleep()
		runtime.Gosched()
	}
	wg.Done()
}

func (p *Philo) deathChecker() {
	for {
		if time.Since(p.timer).Milliseconds() > int64(p.table.timeToDead) {
			fmt.Println(time.Since(p.timer).Milliseconds(), int64(p.table.timeToDead))
			p.typing("is dead", true)
		}
		//time.Sleep(time.Duration(500 * time.Microsecond))
		runtime.Gosched()
	}
}

func worker(philo *Philo, wg *sync.WaitGroup) {
	philo.timer = time.Now()
	go philo.deathChecker()
	if philo.id%2 == 0 {
		go philoCycle(philo, wg)
	} else {
		if philo.id == philo.table.philoCount {
			time.Sleep(1 * time.Millisecond)
		}
		time.Sleep(1 * time.Millisecond)
		go philoCycle(philo, wg)
	}
}

func mid() {
	table := Table{}
	table.timeToDead = 10000
	table.timeToSleep = 2
	table.timeToEat = 2
	table.philoCount = 2
	table.mealsCount = 10
	table.philos = make([]Philo, 2)
	table.start = time.Now()

	initPhilos(&table)

	wg := &sync.WaitGroup{}

	for i := 0; i < table.philoCount; i++ {
		wg.Add(1)
		worker(&table.philos[i], wg)
	}
	wg.Wait()
	fmt.Printf("| %010v |\t%-33s|\n", time.Since(table.start).Milliseconds(), "all philos are full")
}

func main() {
	args := os.Args

	if !(len(args) == 6 || len(args) == 5) {
		log.Fatalln("Wrong count of args")
	}

	table := initTable(args)
	fmt.Println(table)

	initPhilos(&table)

	wg := &sync.WaitGroup{}

	for i := 0; i < table.philoCount; i++ {
		wg.Add(1)
		worker(&table.philos[i], wg)
	}
	wg.Wait()
	fmt.Printf("| %010v |\t%-33s|\n", time.Since(table.start).Milliseconds(), "all philos are full")
}
