from demo_python_pkg.person_node import PersonNode

class WriterNode(PersonNode):
    def __init__(self,book:str) -> None:
        super(). __init__('法外狂徒张三',18)
        print("WriterNode 的 __init__ 方法被调用")
        self.book=book
def main():
    node=WriterNode('西游记')
    node.eat('鱼香肉丝')