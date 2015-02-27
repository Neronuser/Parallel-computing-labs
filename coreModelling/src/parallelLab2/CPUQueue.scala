package parallelLab2
import scala.collection.mutable._

class CPUQueue[A](val id: Int) extends Queue[A] {
  override def toString():String = "Queue #"+id+" : "+mkString(",")
}