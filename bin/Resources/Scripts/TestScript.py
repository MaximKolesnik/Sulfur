import sulfur

class TestScript:
    some_property = 1
    def A(self):
        print(self.some_property)

a = TestScript()
sulfur.RegisterScript(TestScript)
a.A()
