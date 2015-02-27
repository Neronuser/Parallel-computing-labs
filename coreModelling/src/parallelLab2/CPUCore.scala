package parallelLab2

class CPUCore(val id: Int,val queue1:CPUQueue[myProcess],val queue2:CPUQueue[myProcess],var time:Int,val third: Boolean) extends Runnable {
  var runningProcess:myProcess = new myProcess(0,0)
  var currentQueue:CPUQueue[myProcess] = queue1
  var first: Boolean = false
  def run() = {
    Thread sleep 1000
    while(true) {
      if(third) {
        currentQueue = queue1
        if(currentQueue.isEmpty) currentQueue = queue2
      }
      
      currentQueue.synchronized {
        if(!currentQueue.isEmpty){
          first = true
          runningProcess = currentQueue.dequeue()
          println(toString)
        }
        
      }
      if(first){
        Thread sleep time
        first = false
      }
    }
  }
  override def toString():String = "CPUCore #" + id + " runs process: " + runningProcess
}