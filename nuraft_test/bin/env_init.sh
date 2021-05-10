mkfifo calc1.pipe
mkfifo calc2.pipe
mkfifo calc3.pipe
sleep infinity > calc1.pipe &
sleep infinity > calc2.pipe &
sleep infinity > calc3.pipe &