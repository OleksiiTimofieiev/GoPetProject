package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
)

type Data struct {
	IP string `json:"IP"`
	GW string `json:"GW"`
	NM string `json:"NM"`
	L  string `json:"L"`
	T  string `json:"T"`
	H  string `json:"H"`
	P  string `json:"P"`
}

// TODO: wait ten second if zero values received
// TODO: makefile to run app in the Docker container
// TODO: check errors
// TODO: https://ieftimov.com/post/four-steps-daemonize-your-golang-programs/

func main() {
	MakeRequest()
	// for {

	// }
}

// TODO: find ESP32
func MakeRequest() {
	resp, err := http.Get("http://192.168.8.108/data")
	if err != nil {
		log.Fatalln(err)
	}

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		log.Fatalln(err)
	}
	var data Data

	json.Unmarshal([]byte(body), &data)
	// fmt.Println(data)
	fmt.Printf("IP: %s\nGW: %s\nNM: %s\nL: %s lMn\nT: %s C\nH: %s%%\nP: %shPa\n", data.IP, data.GW, data.NM, data.L, data.T, data.H, data.P)
}
