import { sveltekit } from '@sveltejs/kit/vite';
import tailwindcss from 'tailwindcss';
import autoprefixer from 'autoprefixer';

/**
 * @type {import('vite').UserConfig}
 */
export default {
    plugins: [sveltekit()],
    css: {
        postcss: {
            plugins: [tailwindcss(), autoprefixer()]
        }
    }
};
