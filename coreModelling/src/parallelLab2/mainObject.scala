package parallelLab2
object mainObject {
  def main(args: Array[String]) {
      val firstQueue = new CPUQueue[myProcess](1)
      val secondQueue = new CPUQueue[myProcess](2)
      val myFirstThread = new Thread(new myThread(1,2000,firstQueue))
      myFirstThread.start
      val mySecondThread = new Thread(new myThread(2,3000,secondQueue))
      mySecondThread.start
      val firstCore = new Thread(new CPUCore(1,firstQueue,secondQueue,9000,false))
      firstCore.start
      val secondCore = new Thread(new CPUCore(2,secondQueue,firstQueue,5000,false))
      secondCore.start
      val thirdCore = new Thread(new CPUCore(3,firstQueue,secondQueue,3000,true))
      thirdCore.start
    }
}