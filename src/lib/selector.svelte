<script lang="ts">
	export let name: string;
</script>

<h2>{name} selector</h2>
<select {name} id={name}>
	<option value="Major">Major</option>
	<option value="Minor">Minor</option>
	<option value="Pentatonic">Pentatonic</option>
</select>
