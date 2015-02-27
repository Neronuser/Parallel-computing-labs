package parallelLab2

class myProcess(val id: Int, val threadId: Int) {
  override def toString():String = "("+id+","+threadId+")"
}