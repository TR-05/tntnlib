import asyncio
import client

async def main():
    await asyncio.gather(
        client.hello(),
        client.update_data()
    )

asyncio.run(main())