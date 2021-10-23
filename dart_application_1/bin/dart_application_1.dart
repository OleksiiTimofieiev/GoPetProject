import 'package:dart_application_1/dart_application_1.dart'
    as dart_application_1;

void main(List<String> arguments) {
  print('Hello world: ${dart_application_1.calculate()}!');
  //// output
  print('Hello Dart');
  print("Hello '-'\'-\\\nDart");
  //// variables declaration
  var num = 0;
  var word = "hello";
  print("--------------------------");
  print(num);
  print(word);
  print('Var $num.bla-bla-bla ${word.toUpperCase()}');
  dynamic data = "asd";
  data = true;
  print(data);
  print("--------------------------");
  const test = 1;
  print(test);
  print("--------------------------");
  var a = 15, b = 5;
  print("/ : ${(a / b).toInt()}");
  print("--------------------------");
  var res = a == 5 ? 10 : 12;
  print(res);
  print("--------------------------");
  var list = []; // list
  // list.add("asdf");
  list.add(7);
  list.addAll([4, 54, 32]);
  list.removeWhere((element) => element >= 5);
  // first, last, length
  List<int> nums2;
  nums2 = [2, 3, 4];
  print(nums2);
  // also set is available
  print(list);
  print(sum(2, 3));

  //
  // for (classic)
  // for (x in collection)
  // nums2.forEach((element) { })
  var bob = User('Bob', 40, true, ['huh', 'huh']);
  // ..name = 'Bob'
  // ..age = 40;
  bob.info();
}

int sum(a, b) => a + b;
dynamic summ(a, b) => a + b;

class User {
  String Name = "";
  int Age = 0;
  bool isHappy = false;
  List<String> Hobbies = [""];

  User(name, [age, isHappy, hobbies]) {
    this.Name = name;
    this.Age = age;
    this.isHappy = isHappy;
    this.Hobbies = hobbies;
  }

  void info() {
    for (var el in Hobbies) {
      print(el);
    }
  }
}
