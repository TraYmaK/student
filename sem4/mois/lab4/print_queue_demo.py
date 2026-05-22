import asyncio
import json

from spade.agent import Agent
from spade.behaviour import CyclicBehaviour, OneShotBehaviour
from spade.message import Message

DISPATCHER_JID = "dispatcher@localhost"
DISPATCHER_PASSWORD = "pass"
PRINTER_1_JID = "printer1@localhost"
PRINTER_2_JID = "printer2@localhost"
PRINTER_PASSWORD = "pass"
CLIENT_JID = "client@localhost"
CLIENT_PASSWORD = "pass"


class DispatcherAgent(Agent):
    async def setup(self):
        self.queue = []
        self.next_job_id = 1
        self.printer_busy = {
            PRINTER_1_JID: False,
            PRINTER_2_JID: False,
        }
        self.add_behaviour(self.DispatchBehaviour())
        print("[Dispatcher] Started")

    class DispatchBehaviour(CyclicBehaviour):
        async def run(self):
            msg = await self.receive(timeout=1)
            if msg:
                msg_type = msg.get_metadata("type")

                if msg_type == "submit":
                    payload = json.loads(msg.body)
                    job = {
                        "job_id": self.agent.next_job_id,
                        "document_name": payload["document_name"],
                    }
                    self.agent.next_job_id += 1
                    self.agent.queue.append(job)
                    print(f"[Dispatcher] Added job #{job['job_id']}: {job['document_name']}")

                if msg_type == "done":
                    printer_jid = str(msg.sender).split("/")[0]
                    self.agent.printer_busy[printer_jid] = False
                    print(f"[Dispatcher] Printer {printer_jid} is free")

            await self.send_jobs_if_possible()

        async def send_jobs_if_possible(self):
            for printer_jid in [PRINTER_1_JID, PRINTER_2_JID]:
                if not self.agent.queue:
                    return
                if self.agent.printer_busy[printer_jid]:
                    continue
                job = self.agent.queue.pop(0)
                self.agent.printer_busy[printer_jid] = True

                out = Message(to=printer_jid)
                out.set_metadata("type", "job")
                out.body = json.dumps(job)
                await self.send(out)
                print(f"[Dispatcher] Sent job #{job['job_id']} to {printer_jid}")


class PrinterAgent(Agent):
    def __init__(self, jid, password, printer_title):
        super().__init__(jid, password)
        self.printer_title = printer_title

    async def setup(self):
        self.add_behaviour(self.PrintBehaviour())
        print(f"[{self.printer_title}] Started")

    class PrintBehaviour(CyclicBehaviour):
        async def run(self):
            msg = await self.receive(timeout=1)
            if not msg or msg.get_metadata("type") != "job":
                return

            job = json.loads(msg.body)
            print(
                f"[{self.agent.printer_title}] "
                f"Printing #{job['job_id']}: {job['document_name']}"
            )
            await asyncio.sleep(2)
            print(f"[{self.agent.printer_title}] Done #{job['job_id']}")

            done = Message(to=DISPATCHER_JID)
            done.set_metadata("type", "done")
            done.body = json.dumps({"job_id": job["job_id"]})
            await self.send(done)


class ClientAgent(Agent):
    async def setup(self):
        self.add_behaviour(self.SubmitBehaviour())

    class SubmitBehaviour(OneShotBehaviour):
        async def run(self):
            for document_name in ["doc_1.pdf", "doc_2.pdf", "doc_3.pdf", "doc_4.pdf"]:
                msg = Message(to=DISPATCHER_JID)
                msg.set_metadata("type", "submit")
                msg.body = json.dumps({"document_name": document_name})
                await self.send(msg)
                print(f"[Client] Submitted {document_name}")
                await asyncio.sleep(0.2)

            await asyncio.sleep(8)
            await self.agent.stop()


async def main():
    dispatcher = DispatcherAgent(DISPATCHER_JID, DISPATCHER_PASSWORD)
    printer_1 = PrinterAgent(PRINTER_1_JID, PRINTER_PASSWORD, "Printer-1")
    printer_2 = PrinterAgent(PRINTER_2_JID, PRINTER_PASSWORD, "Printer-2")
    client = ClientAgent(CLIENT_JID, CLIENT_PASSWORD)

    await dispatcher.start(auto_register=False)
    await printer_1.start(auto_register=False)
    await printer_2.start(auto_register=False)
    await client.start(auto_register=False)

    while client.is_alive():
        await asyncio.sleep(1)

    await dispatcher.stop()
    await printer_1.stop()
    await printer_2.stop()


if __name__ == "__main__":
    asyncio.run(main())
