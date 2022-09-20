<script lang="ts">
  import {socketStore} from '$lib/stores/socket'
	import { get } from 'svelte/store';
  function handleClick(event: MouseEvent) {
    let socket = get(socketStore)
    socket.send("ping")
  }
  
</script>

<svelte:head>
	<title>Home</title>
	<meta name="description" content="Svelte demo app" />
</svelte:head>

<section>
	<button on:click|preventDefault={handleClick}>ping</button>
</section>

<style>
	section {
		display: flex;
		flex-direction: column;
		justify-content: center;
		align-items: center;
		flex: 1;
	}

</style>
