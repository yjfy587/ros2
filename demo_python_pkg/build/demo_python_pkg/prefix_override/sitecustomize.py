import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/yjfy/桌面/workspace/demo_python_pkg/install/demo_python_pkg'
