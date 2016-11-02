import sulfur

class TestScript(sulfur.Script):
    def __init__(self):
        try:
            super(TestScript, self).__init__()
            self.RegisterOnInitialize(TestScript.OnInitialize)
            self.RegisterOnUpdate(TestScript.OnUpdate)
        except Exception as inst:
            print(inst)
        
    def OnInitialize(self):
        print("Initialize TestScript")

    def OnUpdate(self):
        print(self.owner)

sulfur.RegisterScript(TestScript)

