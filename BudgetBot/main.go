package main

import (
	"bytes"
	"configs"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"models"
	"net/http"
	"strconv"
)

func main() {
	configs, err := configs.ReadConfigs()
	if err != nil {
		// TODO: move to log
		fmt.Println("Configs not parsed")
	}
	botAPI := "https://api.telegram.org/bot"
	fmt.Println(configs.BotToken)
	botURL := botAPI + configs.BotToken
	offset := 0
	// TODO: to log file
	// fmt.Println("Started Bot")

	for {
		updates, err := getUpdates(botURL, offset)
		if err != nil {
			fmt.Println("Not possible to get updates", err.Error())
		}
		if updates == nil {
			continue
		}
		for _, update := range updates {
			// TODO: add routing
			// TODO: if db processed -> return succesful response
			fmt.Printf("%q", update.Message.Text)
			if update.Message.Text == "питание" {
				fmt.Println("Food")
			}
			err = respond(botURL, update)
			offset = update.UpdateId + 1
		}
	}
}

func getUpdates(botURL string, offset int) ([]models.Update, error) {
	resp, err := http.Get(botURL + "/getUpdates" + "?offset=" + strconv.Itoa(offset) + "&timeout=10")
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()
	body, err := ioutil.ReadAll(resp.Body)

	var restResponse models.RestResponse
	err = json.Unmarshal(body, &restResponse)
	if err != nil {
		return nil, err
	}
	return restResponse.Result, nil
}

func respond(botURL string, update models.Update) error {
	var botMessage models.BotMessage

	botMessage.ChatId = update.Message.Chat.ChatId
	botMessage.Text = update.Message.Text

	buf, err := json.Marshal(botMessage)
	if err != nil {
		return err
	}
	_, err = http.Post(botURL+"/sendMessage", "application/json", bytes.NewBuffer(buf))
	if err != nil {
		return err
	}

	return nil
}
