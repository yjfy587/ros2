class PersonNode:
    def __init__(self,name:str,age:int) -> None:
        print("PersonNode 的 __init__ 方法被调用")
        self.name=name
        self.age=age
    def eat(self,food_name:str):
        print(f"{self.name}正在吃{food_name}")
def main():
    node=PersonNode('法外狂徒张三',18)
    node.eat('鱼香肉丝')