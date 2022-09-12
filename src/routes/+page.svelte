<script lang="ts">
	import Selector from "$lib/selector.svelte";
</script>

<div class="grid-container">
	<div class="grid-item">
		<Selector name="scale" />
	</div>
	<div class="grid-item">
    <Selector name="tempo" />
  </div>
	<div class="grid-item">
    <Selector name="note range" />
  </div>

</div>
<h1>Welcome to SvelteKit</h1>
<p>Visit <a href="https://kit.svelte.dev">kit.svelte.dev</a> to read the documentation</p>

<style>
	.grid-container {
		display: inline-grid;
		/* grid-column: 1; */
		grid-template-columns: auto auto auto;
		gap: 2.5rem;
	}
	.grid-item {
		padding: 20px;
		text-align: center;
		border: 1px solid;
	}
</style>
