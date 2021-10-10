package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
)

type Data struct {
	IP string
	GW string
	NM string
}

// TODO: wait ten second if zero values received

func main() {
	MakeRequest()
}

func MakeRequest() {
	resp, err := http.Get("http://192.168.8.103/data")
	if err != nil {
		log.Fatalln(err)
	}

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		log.Fatalln(err)
	}
	var data Data

	json.Unmarshal([]byte(body), &data)
	fmt.Printf("IP: %s, GW: %s", data.IP, data.GW)
}
