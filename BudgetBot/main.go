package main

// TODO: set commands - BotFather

import (
	"fmt"
	"log"

	tgbotapi "github.com/go-telegram-bot-api/telegram-bot-api"
	"github.com/jinzhu/gorm"
	_ "github.com/jinzhu/gorm/dialects/postgres"
)

var numericKeyboard = tgbotapi.NewReplyKeyboard(
	tgbotapi.NewKeyboardButtonRow(
		tgbotapi.NewKeyboardButton("1"),
		tgbotapi.NewKeyboardButton("2"),
		tgbotapi.NewKeyboardButton("3"),
	),
	tgbotapi.NewKeyboardButtonRow(
		tgbotapi.NewKeyboardButton("4"),
		tgbotapi.NewKeyboardButton("5"),
		tgbotapi.NewKeyboardButton("6"),
	),
)

func main() {

	bot, err := tgbotapi.NewBotAPI("2058339968:AAHZQW1uW_Pe1dU7vGrJC-QFSIxf5RHAJt8")
	if err != nil {
		log.Panic(err)
	}
	fmt.Println("here")
	// TODO: create same role on linux
	// TODO: add requests to db
	db, err = gorm.Open("postgres", "user=otimofie dbname=budget password=K4rmU78B2021 host=localhost sslmode=disable")

	if err != nil {

		panic("failed to connect database")

	}

	bot.Debug = false

	// log.Printf("Authorized on account %s", bot.Self.UserName)

	u := tgbotapi.NewUpdate(0)
	u.Timeout = 60

	updates, err := bot.GetUpdatesChan(u)

	for update := range updates {
		if update.Message == nil {
			continue
		}

		if update.Message.IsCommand() {
			msg := tgbotapi.NewMessage(update.Message.Chat.ID, "")
			switch update.Message.Command() {
			case "help":
				msg.Text = "type /sayhi or /status."
			case "sayhi":
				msg.Text = "Hi :)"
			case "status":
				msg.Text = "I'm ok."
			case "withArgument":
				msg.Text = "You supplied the following argument: " + update.Message.CommandArguments()
			case "html":
				msg.ParseMode = "html"
				msg.Text = "This will be interpreted as HTML, click <a href=\"https://www.example.com\">here</a>"
			default:
				msg.Text = "I don't know that command"
			}
			bot.Send(msg)
		}
		msg := tgbotapi.NewMessage(update.Message.Chat.ID, "sdsdfsdfdf")

		switch update.Message.Text {
		case "open":
			msg.ReplyMarkup = numericKeyboard
		}

		bot.Send(msg)
	}

}
