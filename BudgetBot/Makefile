linux:
	@ go build .
arm:
	@ env GOOS=linux GOARCH=arm GOARM=5 go build .
	@ scp BudgetBot otimofie@192.168.8.107:~/
rm:
	@ rm BudgetBot

