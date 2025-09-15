// Wait for DOM to be fully loaded
document.addEventListener('DOMContentLoaded', function() {
    
    // Get all interactive elements
    const loginForm = document.getElementById('login-form');
    const usernameInput = document.getElementById('username');
    const passwordInput = document.getElementById('password');
    const primaryBtns = document.querySelectorAll('.primary-btn');
    const secondaryBtns = document.querySelectorAll('.secondary-btn');
    const textBtns = document.querySelectorAll('.text-btn');
    
    // Add ripple effect to buttons
    function createRipple(event) {
        const button = event.currentTarget;
        const circle = document.createElement('span');
        const diameter = Math.max(button.clientWidth, button.clientHeight);
        const radius = diameter / 2;
        
        circle.style.width = circle.style.height = `${diameter}px`;
        circle.style.left = `${event.clientX - button.offsetLeft - radius}px`;
        circle.style.top = `${event.clientY - button.offsetTop - radius}px`;
        circle.classList.add('ripple');
        
        circle.style.position = 'absolute';
        circle.style.borderRadius = '50%';
        circle.style.background = 'rgba(255, 255, 255, 0.4)';
        circle.style.transform = 'scale(0)';
        circle.style.animation = 'ripple 0.6s linear';
        circle.style.pointerEvents = 'none';
        
        const ripple = button.getElementsByClassName('ripple')[0];
        if (ripple) {
            ripple.remove();
        }
        
        button.appendChild(circle);
    }
    
    // Add CSS animation for ripple effect
    const style = document.createElement('style');
    style.textContent = `
        @keyframes ripple {
            to {
                transform: scale(4);
                opacity: 0;
            }
        }
        /* ... (rest of your CSS animations) ... */
    `;
    document.head.appendChild(style);
    
    // ... (rest of your existing UI enhancement code) ...
    
    // Handle signup form if on signup page
    const signupForm = document.getElementById('signup-form');
    if (signupForm) {
        signupForm.addEventListener('submit', function(e) {
            e.preventDefault(); // Prevent default form submission
            
            const signupSubmitBtn = document.querySelector('.signup-submit-btn');
            const originalText = signupSubmitBtn.querySelector('.btn-text').textContent;

            // Show loading state
            signupSubmitBtn.querySelector('.btn-text').textContent = 'Creating Account...';
            signupSubmitBtn.disabled = true;

            const formData = new FormData(this);
            const data = Object.fromEntries(formData.entries());

            // Send data to the server
            fetch('/signup', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify(data),
            })
            .then(response => response.json())
            .then(result => {
                if (result.success) {
                    signupSubmitBtn.querySelector('.btn-text').textContent = '✓ Account Created!';
                    signupSubmitBtn.style.background = 'linear-gradient(135deg, #28a745, #20c997)';
                    
                    setTimeout(() => {
                        alert(`Welcome to Bank of Diddy, ${data.fullname}! Your account has been created successfully.`);
                        window.location.href = 'index.html';
                    }, 1000);

                } else {
                    // Show error message from server
                    alert(`Error: ${result.message}`);
                    // Reset button
                    signupSubmitBtn.querySelector('.btn-text').textContent = originalText;
                    signupSubmitBtn.disabled = false;
                }
            })
            .catch(error => {
                console.error('Error:', error);
                alert('An unexpected error occurred. Please try again.');
                // Reset button
                signupSubmitBtn.querySelector('.btn-text').textContent = originalText;
                signupSubmitBtn.disabled = false;
            });
        });
    }

    // ... (rest of your existing button handlers and UI effects) ...
});
