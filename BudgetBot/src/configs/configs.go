package configs

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"os"
)

type Configs struct {
	BotToken string `json:"BotToken"`
}

func ReadConfigs() (*Configs, error) {
	jsonConfigsFile, err := os.Open("./src/configs/configs.json")
	if err != nil {
		// TODO: to log file
		fmt.Println(err)
	}

	defer jsonConfigsFile.Close()

	byteValue, _ := ioutil.ReadAll(jsonConfigsFile)

	var configs *Configs

	json.Unmarshal(byteValue, &configs)

	return configs, err
}
